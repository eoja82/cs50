from django.contrib.auth.models import AbstractUser
from django.db import models
from django.db.models.deletion import CASCADE


class User(AbstractUser):
    pass

class Post(models.Model):
    user = models.ForeignKey(User, on_delete=CASCADE)
    post = models.CharField(max_length=250)
    date_created = models.DateTimeField(auto_now_add=True)