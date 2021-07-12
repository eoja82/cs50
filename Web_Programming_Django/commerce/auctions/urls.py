from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("register", views.register, name="register"),
    path("auction/new", views.new_auction, name="new_auction"),
    path("listings/<int:listing_id>", views.listing_view, name="listing_view"),
    path("listings/<int:listing_id>/watch", views.watch, name="watch"),
    path("categories", views.categories, name="categories"),
    path("categories/<str:category>", views.category_view, name="category_view")
]