// follow / unfollow user
document.getElementById("follow").addEventListener("submit", handleFollow)

function handleFollow(e) {
  const follow = e.target.elements["following"].value
  console.log(follow)

  fetch(`/profile/${follow}`, {
    method: "POST",
    body: JSON.stringify({
        follow: follow
    })
  })
  .then( res => {
    if (res.status === 201) return res.json()
  })
  .then( data => {
    console.log(data.followers, data.result)
    document.getElementById("followers").innerText = data.followers
    document.getElementById("follow-btn").innerText = data.result
  })

  e.preventDefault()
}