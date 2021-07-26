import json
from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.decorators import login_required
from django.urls.conf import path
from django.views.decorators.csrf import csrf_exempt

from .models import Like, Post, User


def index(request):
    data = []
    user_likes = False
    try:
        posts = Post.objects.all().order_by("-date_created")
        for post in posts:
            try:
                liked = Like.objects.get(user=request.user, post=post)
                if liked: user_likes = True
                
            except Like.DoesNotExist:
                print("DoesNotExist")
                user_likes = False

            data.append({"post": post, "likes": post.likes.all().count(), "user_likes": user_likes})
    except:
        print("no posts")
        data = []
        
    return render(request, "network/index.html", {
        "data": data, 
    })


def profile(request, user):
    data = []
    user_likes = False
    
    try:
        user_profile = User.objects.get(username=user)
        try:
            posts = Post.objects.filter(user=user_profile).order_by("-date_created")
            
            for post in posts:
                try:
                    liked = Like.objects.get(user=request.user, post=post)
                    if liked: user_likes = True
                    
                except Like.DoesNotExist:
                    print("DoesNotExist")
                    user_likes = False

                data.append({"post": post, "likes": post.likes.all().count(), "user_likes": user_likes})
        except Post.DoesNotExist:
            print("no posts")
            data = []
    except User.DoesNotExist:
        print("user does not exist")
        pass
    

    return render(request, "network/profile.html", {
        "data": data, "profile": user 
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
            return render(request, "network/index.html")
        except:
            pass
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
        return render(request, "network/index.html")


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
