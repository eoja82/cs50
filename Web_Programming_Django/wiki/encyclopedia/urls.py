from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("wiki/<str:wiki>", views.wikipage, name="wikipage"),
    path("newWiki/", views.newWiki, name="newWiki"),
    path("wikiEdit/<str:wiki>", views.wikiEdit, name="wikiEdit")
]