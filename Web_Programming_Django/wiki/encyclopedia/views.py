import markdown
from django.shortcuts import render
from django.http import HttpResponseNotFound

from . import util

def index(request):
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
        return HttpResponseNotFound(f"<h1 style='text-align: center; margin: 3rem;'>Sorry, there is no information available for \"{wiki.lower()}\".</h1>")