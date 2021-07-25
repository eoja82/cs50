// like / unlike a post
const likes = document.querySelectorAll(".like")

likes.forEach( like => like.addEventListener("click", handleLike))

function handleLike(e) {
  const data = e.target.dataset

  if (data.post_user !== data.user) {
    fetch("/posts/like", {
      method: "POST",
      body: JSON.stringify({
        post_id: data.post_id
      })
    })
    .then( res => {
      if (res.status === 201) return res.json()
    })
    .then( data => {
      e.target.firstElementChild.innerText = `(${data.likes})`
    })
    .catch( err => console.log("Error", err))
  }

  e.preventDefault()
}

// show / hide edit post form
const edits = document.querySelectorAll(".edit")
edits.forEach( edit => edit.addEventListener("click", editForm))

function editForm(e) {
  const id = e.target.dataset.post_id,
        editPost = document.querySelector(`#edit-post-${id}`),
        post = document.querySelector(`#post-${id}`)

  if (editPost.style.display === "none") {
    editPost.style.display = "block"
    post.style.display = "none"
    e.target.innerText = "Cancel"
  } else {
    editPost.style.display = "none"
    post.style.display = "block"
    e.target.innerText = "Edit"
  }
}

// edit a post
const editPosts = document.querySelectorAll(".edit-post-form")
editPosts.forEach( form => form.addEventListener("submit", editPost))

function editPost(e) {
  const data = e.target.elements,
        id = data["post-id"].value,
        edit = data["edit-post"].value

  fetch('/post', {
    method: "PUT",
    body: JSON.stringify({
      post_id: id,
      edit: edit
    })
  })
  .then( res => res.json())
  .then( data => {
    document.getElementById(`post-${id}`).innerText = data.post
    document.getElementById(`edit-${id}`).innerText = data.post
    document.getElementById(`show-edit-${id}`).click()
  })
  .catch( err => console.log("Error:", err)) 

  e.preventDefault()
}