# Generated by Django 3.2.4 on 2021-07-28 00:12

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('network', '0006_auto_20210727_2352'),
    ]

    operations = [
        migrations.RenameField(
            model_name='follow',
            old_name='fallowing',
            new_name='following',
        ),
    ]
