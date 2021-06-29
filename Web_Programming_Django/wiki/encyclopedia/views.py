from django.shortcuts import render

from . import util


def index(request):
    return render(request, "encyclopedia/index.html", {
        "entries": util.list_entries()
    })

def wikipage(request, wiki):
    return render(request, "encyclopedia/wikipage.html", {
        "wiki": wiki.capitalize(), "file": util.get_entry(wiki.lower())
    })