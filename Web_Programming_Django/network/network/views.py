import json
from django.contrib.auth import authenticate, login, logout
from django.core import paginator
from django.db import IntegrityError
from django.http import HttpResponse, HttpResponseNotFound, HttpResponseRedirect, HttpResponseServerError, JsonResponse
from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.decorators import login_required
from django.urls.conf import path
from django.views.decorators.csrf import csrf_exempt
from django.core.paginator import Paginator

from .models import Follow, Like, Post, User


def index(request):
    data = []
    user_likes = False
    try:
        posts = Post.objects.all().order_by("-date_created")
        for post in posts:
            if not request.user.is_anonymous:
                try:
                    liked = Like.objects.get(user=request.user, post=post)
                    if liked: user_likes = True
                    
                except Like.DoesNotExist:
                    user_likes = False

            data.append({"post": post, "likes": post.likes.all().count(), "user_likes": user_likes})
    except Exception as e:
        print(e)
        print("no posts")
        data = []
    
    paginator = Paginator(data, 10)
    page_number = request.GET.get("page")
    page_obj = paginator.get_page(page_number)
        
    return render(request, "network/index.html", {
        "data": page_obj, 
    })

@csrf_exempt
def profile(request, profile):
    """ renders a user's profile page, allows authenticated users to follow and unfollow other users """

    if request.method == "POST":
        data = json.loads(request.body)
        result = None
        
        # check if user to follow exists
        try:
            user_to_follow = User.objects.get(username=data["follow"])
            
            # check if user is already following
            try:
                # unfollow if found
                following = Follow.objects.get(following=user_to_follow, follower=request.user)
                following.delete()
                result = "Follow"

            except Follow.DoesNotExist:
                # not found follow 
                follow = Follow(following=user_to_follow, follower=request.user)
                follow.save()
                result = "Unfollow"

            followers = user_to_follow.followers.all().count()

        except User.DoesNotExist:
            # user to follow does not exist
            pass

        return JsonResponse({"followers": followers, "result": result}, status=201)

    else:
        data = []
        user_likes = False
        user_following = False
        
        # check if user exists, number following and followers
        try:
            user_profile = User.objects.get(username=profile)
            followers = user_profile.followers.all().count()
            following = user_profile.following.all().count()

            # check if user is following
            try:
                followed = Follow.objects.get(following=user_profile, follower=request.user)
                user_following = True
            except Follow.DoesNotExist:
                user_following = False

            # check if user made any posts
            try:
                posts = Post.objects.filter(user=user_profile).order_by("-date_created")
                
                for post in posts:
                    try:
                        liked = Like.objects.get(user=request.user, post=post)
                        if liked: user_likes = True
                        
                    except Like.DoesNotExist:
                        user_likes = False

                    data.append({"post": post, "likes": post.likes.all().count(), "user_likes": user_likes})

            except Post.DoesNotExist:
                print("no posts")
                data = []

        except User.DoesNotExist:
            print("user does not exist")
            pass
        
        paginator = Paginator(data, 10)
        page_number = request.GET.get("page")
        page_obj = paginator.get_page(page_number)

        return render(request, "network/profile.html", {
            "data": page_obj, "profile": profile, "followers": followers, "following": following, "user_following": user_following
        })


@login_required
def following(request):
    data = []
    
    # check if user exists and get followers
    try:
        user = User.objects.get(id=request.user.id)
        following = user.following.all()
        
    except User.DoesNotExist:
        print("user does not exist")
        return HttpResponseNotFound()
    
    # get all posts filter out not followed
    try:
        posts = Post.objects.all().order_by("-date_created")
        for post in posts:
            for followed in following:
                if followed.following == post.user:
                    data.append({"post": post, "likes": post.likes.all().count()})

    except Post.DoesNotExist:
        print("no posts")

    paginator = Paginator(data, 10)
    page_number = request.GET.get("page")
    page_obj = paginator.get_page(page_number)

    return render(request, "network/following.html", {
        "data": page_obj
    })


@csrf_exempt
@login_required
def post(request):
    """ create new post or edit existing post """

    # create new post
    if request.method == "POST":
        try:
            post = Post(user=request.user, post=request.POST["new-post"])
            post.save()
            print("new post saved")
            return HttpResponseRedirect(reverse("index"))
        except:
            return HttpResponseServerError()
    # edit post
    elif request.method == "PUT":
        data = json.loads(request.body)
        id = data["post_id"]
        edit = data["edit"]

        try:
            post = Post.objects.get(id=id)
            post.post = edit
            post.save()
            print("Post edited, return json")
            return JsonResponse({"post": post.post}, status=201)
        except Post.DoesNotExist:
            pass
    else:
        return HttpResponseRedirect(reverse("index"))


@csrf_exempt
@login_required
def like_post(request):
    """ handle like / unlike of post """

    if request.method == "POST":
        data = json.loads(request.body)
        
        try:
            post = Post.objects.get(id=data["post_id"])
        except Post.DoesNotExist:
            pass

        try:
            # unlike if found
            liked = Like.objects.get(user=request.user, post=post)
            liked.delete()
        except Like.DoesNotExist:
            # add like
            like = Like(user=request.user, post=post)
            like.save()

        likes = post.likes.all().count()

        return JsonResponse({"likes": likes}, status=201)
    else:
        print("like_post accessed via GET")
        return render(request, "network/index.html")


def login_view(request):
    if request.method == "POST":

        # Attempt to sign user in
        username = request.POST["username"]
        password = request.POST["password"]
        user = authenticate(request, username=username, password=password)

        # Check if authentication successful
        if user is not None:
            login(request, user)
            return HttpResponseRedirect(reverse("index"))
        else:
            return render(request, "network/login.html", {
                "message": "Invalid username and/or password."
            })
    else:
        return render(request, "network/login.html")


def logout_view(request):
    logout(request)
    return HttpResponseRedirect(reverse("index"))


def register(request):
    if request.method == "POST":
        username = request.POST["username"]
        email = request.POST["email"]

        # Ensure password matches confirmation
        password = request.POST["password"]
        confirmation = request.POST["confirmation"]
        if password != confirmation:
            return render(request, "network/register.html", {
                "message": "Passwords must match."
            })

        # Attempt to create new user
        try:
            user = User.objects.create_user(username, email, password)
            user.save()
        except IntegrityError:
            return render(request, "network/register.html", {
                "message": "Username already taken."
            })
        login(request, user)
        return HttpResponseRedirect(reverse("index"))
    else:
        return render(request, "network/register.html")
