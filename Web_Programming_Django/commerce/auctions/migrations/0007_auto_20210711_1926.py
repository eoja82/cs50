# Generated by Django 3.2.4 on 2021-07-11 19:26

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('auctions', '0006_auto_20210709_2023'),
    ]

    operations = [
        migrations.CreateModel(
            name='Bid',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('bid', models.DecimalField(decimal_places=2, max_digits=12)),
                ('date_created', models.DateField(auto_now=True)),
                ('listing', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='auctions.listings')),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='bidder', to=settings.AUTH_USER_MODEL)),
            ],
        ),
        migrations.RenameModel(
            old_name='Comments',
            new_name='Comment',
        ),
        migrations.RenameField(
            model_name='comment',
            old_name='comment',
            new_name='comments',
        ),
        migrations.DeleteModel(
            name='Bids',
        ),
    ]
