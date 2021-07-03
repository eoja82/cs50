const classNames = {
  TODO_ITEM: 'todo-container',
  TODO_CHECKBOX: 'todo-checkbox',
  TODO_TEXT: 'todo-text',
  TODO_DELETE: 'todo-delete',
}

const list = document.getElementById('todo-list')
const itemCountSpan = document.getElementById('item-count')
const uncheckedCountSpan = document.getElementById('unchecked-count')

function newTodo() {
  let newTodo = prompt("New TODO")
  console.log(newTodo)
  if (newTodo.length === 0) return
  
  let li = document.createElement("li")
  li.className = "todo-container"
  
  let checkBox = document.createElement("input")
  checkBox.type = "checkbox"
  checkBox.className = "todo-checkbox"
  checkBox.addEventListener("change", updateChecked)

  let p = document.createElement("p")
  p.textContent = newTodo
  p.className = "todo-text"

  li.appendChild(checkBox)
  li.appendChild(p)
  list.appendChild(li)

  itemCountSpan.textContent = Number(itemCountSpan.textContent) + 1
  uncheckedCountSpan.textContent = Number(uncheckedCountSpan.textContent) + 1

  function updateChecked(e) {
    if (e.target.checked) {
      uncheckedCountSpan.textContent = Number(uncheckedCountSpan.textContent) - 1
    } else {
      uncheckedCountSpan.textContent = Number(uncheckedCountSpan.textContent) + 1
    }
  }
}

