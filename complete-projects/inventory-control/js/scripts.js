const itemForm = document.querySelector("#item-form");
const nameInput = document.querySelector("#name-input");
const minimumInput = document.querySelector("#minimum-input");
const quantityInput = document.querySelector("#quantity-input");
const itemList = document.querySelector("#item-list");

const editForm = document.querySelector("#edit-form");
const editNameInput = document.querySelector("#edit-name-input");
const editMinimumInput = document.querySelector("#edit-minimum-input");
const editQuantityInput = document.querySelector("#edit-quantity-input");
const cancelEditBtn = document.querySelector("#cancel-edit");

const switchThemeBtn = document.querySelector("#switch-theme");
const body = document.body;

let oldItemName;
let oldItemMinimum;
let oldItemQuantity;
let itemEl;
let itemInfoEl;

// Functions
const saveItem = (name, minimum, quantity, save = 1) => {
  const item = document.createElement("div");
  item.classList.add("item");

  const itemInfo = document.createElement("div");
  itemInfo.classList.add("item-info");
  const itemName = document.createElement("h3");
  itemName.innerText = name;
  itemInfo.appendChild(itemName);
  const itemMinimum = document.createElement("p");
  itemMinimum.innerHTML = `Min: <span class="min">${minimum}</span>`;
  itemInfo.appendChild(itemMinimum);
  const itemQuantity = document.createElement("p");
  itemQuantity.innerHTML = `Quant: <span class="quant">${quantity}</span>`;
  itemInfo.appendChild(itemQuantity);
  item.appendChild(itemInfo);

  const increaseBtn = document.createElement("button");
  increaseBtn.classList.add("increase-btn");
  increaseBtn.innerHTML = '<i class="fa-solid fa-plus"></i>';
  item.appendChild(increaseBtn);

  const decreaseBtn = document.createElement("button");
  decreaseBtn.classList.add("decrease-btn");
  decreaseBtn.innerHTML = '<i class="fa-solid fa-minus"></i>';
  item.appendChild(decreaseBtn);

  const editBtn = document.createElement("button");
  editBtn.classList.add("edit-btn");
  editBtn.innerHTML = '<i class="fa-solid fa-pen"></i>';
  item.appendChild(editBtn);

  const deleteBtn = document.createElement("button");
  deleteBtn.classList.add("delete-btn");
  deleteBtn.innerHTML = '<i class="fa-solid fa-trash"></i>';
  item.appendChild(deleteBtn);

  itemList.appendChild(item);

  itemEl = item;
  checkBelow(Number(minimum), Number(quantity));

  if (save) {
    saveToLocalStorage({ name, minimum, quantity });
  }

  nameInput.value = "";
  minimumInput.value = "";
  quantityInput.value = "";

  nameInput.focus();
};

const editItem = (newName, newMinimum, newQuantity) => {
  let nameValue = itemInfoEl.querySelector("h3");
  let minimumValue = itemInfoEl.querySelector("p .min");
  let quantityValue = itemInfoEl.querySelector("p .quant");

  if (nameValue.innerText === oldItemName) {
    nameValue.innerText = newName;

    editItemNameLocalStorage(oldItemName, newName);
  }

  if (minimumValue.innerText === oldItemMinimum) {
    minimumValue.innerText = newMinimum;

    editItemMinimumLocalStorage(oldItemMinimum, newMinimum);
  }

  if (quantityValue.innerText === oldItemQuantity) {
    quantityValue.innerText = newQuantity;

    editItemQuantityLocalStorage(oldItemQuantity, newQuantity);
  }

  checkBelow(Number(newMinimum), Number(newQuantity));
};

const checkBelow = (minimum, quantity) => {
  quantity < minimum
    ? itemEl.classList.add("below")
    : itemEl.classList.remove("below");
};

const toggleForms = () => {
  itemForm.classList.toggle("hide");
  itemList.classList.toggle("hide");
  editForm.classList.toggle("hide");
};

// Events
itemForm.addEventListener("submit", (e) => {
  e.preventDefault();

  const nameValue = nameInput.value;
  const minimumValue = minimumInput.value;
  const quantityValue = quantityInput.value;

  if (nameValue && minimumValue && quantityValue) {
    saveItem(nameValue, minimumValue, quantityValue);
  }
});

itemList.addEventListener("click", (e) => {
  targetEl = e.target;
  parentEl = targetEl.closest("div");
  let itemName;
  let itemMinimum;
  let itemQuantity;

  if (parentEl && parentEl.querySelector(".item-info").querySelector("h3")) {
    itemName =
      parentEl.querySelector(".item-info").querySelector("h3").innerText || "";
    itemMinimum =
      parentEl.querySelector(".item-info").querySelector("p .min").innerText ||
      "";
    itemQuantity =
      parentEl.querySelector(".item-info").querySelector("p .quant")
        .innerText || "";
  }

  if (targetEl.classList.contains("increase-btn")) {
    itemQuantity++;

    itemEl = parentEl;
    itemInfoEl = parentEl.querySelector(".item-info");

    checkBelow(itemMinimum, itemQuantity);

    parentEl.querySelector("p .quant").innerText = itemQuantity;

    updateItemQuantLocalStorage(itemQuantity);
  }

  if (targetEl.classList.contains("decrease-btn")) {
    itemQuantity--;

    itemEl = parentEl;
    itemInfoEl = parentEl.querySelector(".item-info");

    checkBelow(itemMinimum, itemQuantity);

    parentEl.querySelector("p .quant").innerText = itemQuantity;

    updateItemQuantLocalStorage(itemQuantity);
  }

  if (targetEl.classList.contains("edit-btn")) {
    toggleForms();

    editNameInput.value = itemName;
    editMinimumInput.value = itemMinimum;
    editQuantityInput.value = itemQuantity;

    itemEl = parentEl;
    itemInfoEl = parentEl.querySelector(".item-info");
    oldItemName = itemName;
    oldItemMinimum = itemMinimum;
    oldItemQuantity = itemQuantity;

    editNameInput.focus();
  }

  if (targetEl.classList.contains("delete-btn")) {
    parentEl.remove();

    itemEl = parentEl.querySelector(".item-info");

    deleteFromLocalStorage(itemEl.querySelector("h3").innerText);
  }
});

cancelEditBtn.addEventListener("click", (e) => {
  e.preventDefault();

  toggleForms();
});

editForm.addEventListener("submit", (e) => {
  e.preventDefault();

  const newName = editNameInput.value;
  const newMinimum = editMinimumInput.value;
  const newQuantity = editQuantityInput.value;

  if (newName || newMinimum || newQuantity) {
    editItem(newName, newMinimum, newQuantity);
  }

  toggleForms();
});

// Local Storage
const getItemsLocalStorage = () => {
  const items = JSON.parse(localStorage.getItem("items")) || [];

  return items;
};

const saveToLocalStorage = (item) => {
  const items = getItemsLocalStorage();

  items.push(item);
  localStorage.setItem("items", JSON.stringify(items));
};

const updateItemQuantLocalStorage = (newQuantity) => {
  const items = getItemsLocalStorage();

  items.map((item) => {
    item.name === itemInfoEl.querySelector("h3").innerText
      ? (item.quantity = newQuantity)
      : null;
  });
  localStorage.setItem("items", JSON.stringify(items));
};

const editItemNameLocalStorage = (oldNameValue, newNameValue) => {
  const items = getItemsLocalStorage();

  items.map((item) => {
    item.name === oldNameValue ? (item.name = newNameValue) : null;
  });
  localStorage.setItem("items", JSON.stringify(items));
};

const editItemMinimumLocalStorage = (oldMinimumValue, newMinimumValue) => {
  const items = getItemsLocalStorage();

  items.map((item) => {
    item.minimum === oldMinimumValue ? (item.minimum = newMinimumValue) : null;
  });
  localStorage.setItem("items", JSON.stringify(items));
};

const editItemQuantityLocalStorage = (oldQuantityValue, newQuantityValue) => {
  const items = getItemsLocalStorage();

  items.map((item) => {
    item.quantity === oldQuantityValue
      ? (item.quantity = newQuantityValue)
      : null;
  });
  localStorage.setItem("items", JSON.stringify(items));
};

const deleteFromLocalStorage = (itemName) => {
  const items = getItemsLocalStorage();

  const filteredItems = items.filter((item) => item.name != itemName);

  localStorage.setItem("items", JSON.stringify(filteredItems));
};

const loadItemsFromLocalStorage = () => {
  const items = getItemsLocalStorage();

  items.forEach((item) => {
    saveItem(item.name, item.minimum, item.quantity, 0);
  });
};

// Theme
const theme = localStorage.getItem("theme");

if (theme) {
  switch (theme) {
    case "light":
      body.classList.add("light");
      switchThemeBtn.innerHTML = '<i class="fa-regular fa-moon"></i>';
      break;

    case "dark":
      body.classList.add("dark");
      switchThemeBtn.innerHTML = '<i class="fa-regular fa-sun"></i>';
      break;
  }
}

switchThemeBtn.addEventListener("click", () => {
  if (body.classList.contains("light")) {
    body.classList.replace("light", "dark");
    localStorage.setItem("theme", "dark");
    switchThemeBtn.innerHTML = '<i class="fa-regular fa-sun"></i>';
  } else if (body.classList.contains("dark")) {
    body.classList.replace("dark", "light");
    localStorage.setItem("theme", "light");
    switchThemeBtn.innerHTML = '<i class="fa-regular fa-moon"></i>';
  }
});

// Init App
loadItemsFromLocalStorage();
