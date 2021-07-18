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
          subjet = data["compose-subject"].value,
          body = data["compose-body"].value

    fetch("/emails", {
      method: "POST",
      body: JSON.stringify({
        recipients: recipients,
        subjet: subjet,
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

  
}