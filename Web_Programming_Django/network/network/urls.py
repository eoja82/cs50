
from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("following/", views.following, name="following"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("posts/like", views.like_post, name="like"),
    path("post", views.post, name="post"),
    path("profile/<str:profile>", views.profile, name="profile"),
    path("register", views.register, name="register")
]