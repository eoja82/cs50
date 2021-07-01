from django.forms import utils
import markdown
from django import forms
from django.shortcuts import render
from django.http import HttpResponseRedirect, HttpResponse
from django.urls import reverse

from . import util

def index(request):
    if request.method == "POST":
        query = request.POST["q"].strip()
        term = util.get_entry(query.lower())
        if term != None:
            return HttpResponseRedirect(reverse("wikipage", args={query}))
        else:
            matches = []
            entries = util.list_entries()
            for entry in entries:
                if entry.lower().find(query.lower()) >= 0:
                    matches.append(entry)
            return render(request, "encyclopedia/searchResults.html", {
                "matches": matches, "query": query
            })
    else:
        return render(request, "encyclopedia/index.html", {
            "entries": util.list_entries()
        })

def wikipage(request, wiki):
    entry = util.get_entry(wiki.lower())
    if entry != None:
        md = markdown.Markdown().convert(entry)
        title = md[md.index(">") + 1:]
        title = title[:title.index("<")]
        return render(request, "encyclopedia/wikipage.html", {
            "file": md, "title": title
        })
    else:
        return render(request, "encyclopedia/notFound.html", {
            "wiki": wiki.lower()
        }, status="404")

def newWiki(request):
    if request.method == "POST":
        pass
    else:
        return render(request, "encyclopedia/newWiki.html")