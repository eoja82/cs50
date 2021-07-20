document.addEventListener('DOMContentLoaded', function() {

  // Use buttons to toggle between views
  document.querySelector('#inbox').addEventListener('click', () => load_mailbox('inbox'));
  document.querySelector('#sent').addEventListener('click', () => load_mailbox('sent'));
  document.querySelector('#archived').addEventListener('click', () => load_mailbox('archive'));
  document.querySelector('#compose').addEventListener('click', compose_email);

  // By default, load the inbox
  load_mailbox('inbox');
});

function compose_email(replyEmail = false) {

  // Show compose view and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#read-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'block';

  if (replyEmail.sender) {
    // pre-fill composition fields
    let subjectPrefix = "Re: "
    const regex = /^Re:\s/i
    if (regex.test(replyEmail.subject)) subjectPrefix = ""
    
    document.querySelector('#compose-recipients').value = replyEmail.sender;
    document.querySelector('#compose-subject').value = `${subjectPrefix} ${replyEmail.subject}`;
    document.querySelector('#compose-body').value = `\nOn ${replyEmail.timestamp} ${replyEmail.sender} wrote:\n\n ${replyEmail.body}`;
  } else {
    // Clear out composition fields
    document.querySelector('#compose-recipients').value = '';
    document.querySelector('#compose-subject').value = '';
    document.querySelector('#compose-body').value = '';
  }

  document.querySelector("#compose-form").addEventListener("submit", sendEmail)

  function sendEmail(e) {
    const data = e.target.elements,
          recipients = data["compose-recipients"].value,
          subject = data["compose-subject"].value,
          body = data["compose-body"].value

    fetch("/emails", {
      method: "POST",
      body: JSON.stringify({
        recipients: recipients,
        subject: subject,
        body: body
      })
    })
    .then( res => {
      if (res.ok) {
        console.log("Message Sent")
        load_mailbox("sent")
      } else {
        alert("Something went wrong.  The message was not sent.")
      }
    })
    .catch( err => console.log("Error:", err))

    e.preventDefault()
  }
}

function load_mailbox(mailbox) {
  
  // Show the mailbox and hide other views
  document.querySelector('#emails-view').style.display = 'block';
  document.querySelector('#read-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'none';

  // Show the mailbox name
  document.querySelector('#emails-view').innerHTML = `<h3>${mailbox.charAt(0).toUpperCase() + mailbox.slice(1)}</h3>`;

  fetch(`/emails/${mailbox}`)
  .then( res => res.json())
  .then( emails => {
    emails.forEach( email => {
      const emailDiv = document.getElementById("emails-view"),
        card = document.createElement("div"),
        cardBody = document.createElement("div"),
        cardTitle = document.createElement("h5"),
        cardSubtitle = document.createElement("h6"),
        preview = document.createElement("P"),
        date = document.createElement("p")
      
      card.className = "card"
      card.style.cursor = "pointer"
      if (email.read) card.style.backgroundColor = "lightgray"
      card.onclick = () => view_email(email, mailbox)
      
      cardBody.className = "card-body"
      cardBody.style.position = "relative"
      card.append(cardBody)
      
      cardTitle.className = "card-title"
      cardTitle.textContent = email.recipients[0]
      cardBody.append(cardTitle)
      
      cardSubtitle.className = "card-subtitle"
      cardSubtitle.textContent = email.subject
      cardBody.append(cardSubtitle)
      
      preview.className = "card-text text-muted"
      preview.textContent = email.body.slice(0, 50)
      cardBody.append(preview)
      
      date.className = "card-text"
      date.style = "position: absolute; bottom: 35px; right: 20px"
      date.textContent = email.timestamp
      cardBody.append(date)

      emailDiv.append(card)
    })
  })
  .catch( err => console.log("Error: ", err))
}

function view_email(email, mailbox) {
  // Show the mailbox and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#read-view').style.display = 'block';
  document.querySelector('#compose-view').style.display = 'none';

  const read = document.getElementById("read-view")
  // clear out previous views
  read.innerHTML = ""

   const sender =document.createElement("p")
   sender.style.margin = "0"
   sender.innerHTML = `<b>From: </b>${email.sender}`
   read.append(sender)

   const recipients = document.createElement("p")
   recipients.style.margin = "0"
   recipients.innerHTML = `<b>To: </b>${email.recipients.join(", ")}`
   read.append(recipients)

   const subject = document.createElement("p")
   subject.style.margin = "0"
   subject.innerHTML = `<b>Subject: </b>${email.subject}`
   read.append(subject)

   const date = document.createElement("p")
   date.style.margin = "0"
   date.innerHTML = `<b>Date: </b>${email.timestamp}`
   read.append(date)

   const responseButtons = document.createElement("div")

  const reply = document.createElement("button")
  reply.className = "btn btn-sm btn-outline-primary"
  reply.style.margin = "5px"
  reply.textContent = "Reply"
  reply.onclick = () => compose_email(email)
  responseButtons.append(reply)

  if (mailbox !== "sent") {
    const archive = document.createElement("button")
    archive.className = "btn btn-sm btn-outline-primary"
    archive.style.margin = "5px"
    if (!email.archived) {
      archive.innerText = "Archive"
    } else {
      archive.innerText = "Unarchive"
    }
    archive.addEventListener("click", archive_email)
    responseButtons.append(archive)
  }

  read.append(responseButtons)

  read.append(document.createElement("hr"))

  const body = document.createElement("p")
  body.innerText = email.body
  read.append(body)

  // mark as archived or unarchive
  function archive_email() {
    fetch(`/emails/${email.id}`, {
      method: "PUT",
      body: JSON.stringify({
        archived: email.archived ? false : true
      })
    })
    .then( res => {
      if (res.ok) {
        load_mailbox("inbox")
      } else {
        console.log("Error archiving email.")
      }
    })
    .catch( err => console.log("Error: ", err))
  }

  // mark as read
  fetch(`/emails/${email.id}`, {
    method: "PUT",
    body: JSON.stringify({
      read: true
    })
  })
  .then( res => {
    if (!res.ok) console.log("Error marking email as read.")
  })
  .catch( err => console.log("Error: ", err))
}