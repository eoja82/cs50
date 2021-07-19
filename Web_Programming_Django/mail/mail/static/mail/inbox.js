document.addEventListener('DOMContentLoaded', function() {

  // Use buttons to toggle between views
  document.querySelector('#inbox').addEventListener('click', () => load_mailbox('inbox'));
  document.querySelector('#sent').addEventListener('click', () => load_mailbox('sent'));
  document.querySelector('#archived').addEventListener('click', () => load_mailbox('archive'));
  document.querySelector('#compose').addEventListener('click', compose_email);

  // By default, load the inbox
  load_mailbox('inbox');
});

function compose_email() {

  // Show compose view and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'block';

  // Clear out composition fields
  document.querySelector('#compose-recipients').value = '';
  document.querySelector('#compose-subject').value = '';
  document.querySelector('#compose-body').value = '';

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
        date = document.createElement("p"),
        read = document.createElement("div")
      console.log(email.subject)
      card.className = "card"
      card.style.cursor = "pointer"
      card.onclick = () => view_email(email)
      
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
      if (!email.read) {
        date.style = "position: absolute; bottom: 18px; right: 20px"
      } else {
        date.style = "position: absolute; bottom: 35px; right: 20px"
      }
      date.textContent = email.timestamp
      cardBody.append(date)

      if (!email.read) {
        read.style = "width: 16px; height: 16px; background-color: #007bff; border-radius: 8px; position: absolute; top: 20px; right: 20px;"
        cardBody.append(read)
      }

      emailDiv.append(card)
    })
  })
  .catch( err => console.log("Error: ", err))
}

function view_email(email) {
  console.log(email)
}