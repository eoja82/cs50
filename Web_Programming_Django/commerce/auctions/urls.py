from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("register", views.register, name="register"),
    path("auction/new", views.new_auction, name="new_auction"),
    path("auction/close/<int:listing_id>", views.close_auction, name="close_auction"),
    path("listings/<int:listing_id>", views.listing_view, name="listing_view"),
    path("listings/user", views.user_listings, name="user_listings"),
    path("listings/won", views.won, name="won"),
    path("listings/comment", views.comment, name="comment"),
    path("listings/<int:listing_id>/watch", views.watch, name="watch"),
    path("watchlist", views.watchlist, name="watchlist"),
    path("categories", views.categories, name="categories"),
    path("categories/<str:category>", views.category_view, name="category_view")
]