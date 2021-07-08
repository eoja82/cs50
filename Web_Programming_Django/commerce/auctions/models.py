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
    title = models.CharField(max_length=100)
    description = models.TextField(max_length=2000)
    photoURL = models.URLField(blank=True)
    category = models.CharField(max_length=15, choices=CATEGORY_CHOICES, blank=True)
    starting_bid = models.DecimalField(max_digits=12, decimal_places=2)
    date_created = models.DateField()

class Bids(models.Model):
    listing = models.ForeignKey(Listings, on_delete=models.CASCADE) # related_name
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    bid = models.DecimalField(max_digits=12, decimal_places=2)
    bidder = models.CharField(max_length=100)
    date_created = models.DateField()

class Comments(models.Model):
    listing = models.ForeignKey(Listings, on_delete=models.CASCADE) # related_name
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    comment = models.TextField(max_length=1000)
    date_created = models.DateField()