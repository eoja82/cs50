from django.contrib.auth.models import AbstractUser
from django.db import models


class User(AbstractUser):
    pass

class Listings(models.Model):
    CATEGORY_CHOICES = [
        ("None", "Optional"),
        ("Home", "Home"),
        ("Auto", "Auto"),
        ("Farm", "Farm"),
        ("Recreation", "Recreation"),
        ("Sporting Goods", "Sporting Goods"),
        ("Watercraft", "Watercraft"),
        ("Electronics", "Electronics")
    ]
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    title = models.CharField(max_length=100)
    description = models.TextField(max_length=2000)
    photoURL = models.URLField(blank=True)
    category = models.CharField(max_length=15, choices=CATEGORY_CHOICES, blank=True)
    starting_bid = models.DecimalField(max_digits=12, decimal_places=2)
    highest_bid = models.DecimalField(max_digits=12, decimal_places=2, blank=True, default=0)
    date_created = models.DateField(auto_now_add=True)

    def __str__(self):
        return f"User: {self.user}, Listing: {self.title}."

class Watch(models.Model):
    listing = models.ManyToManyField(Listings, related_name="watching", blank=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)

    def __str__(self):
        return f"{self.user}, watching: {self.watching}"

class Bid(models.Model):
    listing = models.ForeignKey(Listings, related_name="bids", on_delete=models.CASCADE)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    bids = models.DecimalField(max_digits=12, decimal_places=2)
    date_created = models.DateField(auto_now=True)

    def __str__(self):
        return f"User: {self.user}, Listing: {self.listing}, Bid: {self.bids}"

class Comment(models.Model):
    listing = models.ForeignKey(Listings, related_name="comments", on_delete=models.CASCADE)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    comments = models.TextField(max_length=1000)
    date_created = models.DateField(auto_now=True)

    def __str__(self):
        return f"User: {self.user}, Comment: {self.comment}, Date: {self.date_created}"