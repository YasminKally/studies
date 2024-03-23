const itemForm = document.querySelector('#item-form');
const nameInput = document.querySelector('#name-input');
const minimumInput = document.querySelector('#minimum-input');
const quantityInput = document.querySelector('#quantity-input');
const itemList = document.querySelector('#item-list');

// Functions
const saveItem = (name, minimum, quantity, below = 0, save = 1) => {
    const item = document.createElement('div');
    item.classList.add('item');
    below = checkBelow(minimum, quantity);

    const itemInfo = document.createElement('div');
    itemInfo.classList.add('item-info');
    const itemName = document.createElement('h4');
    itemName.innerText = name;
    itemInfo.appendChild(itemName);
    const itemMinimum = document.createElement('p');   
    itemMinimum.innerHTML = `Min: <span class="min">${minimum}</span>`;
    itemInfo.appendChild(itemMinimum);
    const itemQuantity = document.createElement('p');   
    itemQuantity.innerHTML = `Quant: <span class="quant">${quantity}</span>`;
    itemInfo.appendChild(itemQuantity);
    item.appendChild(itemInfo);

    const increaseBtn = document.createElement('button');
    increaseBtn.classList.add('increase-btn');
    increaseBtn.innerHTML = '<i class="fa-solid fa-plus"></i>';
    item.appendChild(increaseBtn);

    const decreaseBtn = document.createElement('button');
    decreaseBtn.classList.add('decrease-btn');
    decreaseBtn.innerHTML = '<i class="fa-solid fa-minus"></i>';
    item.appendChild(decreaseBtn);

    const editBtn = document.createElement('button');
    editBtn.classList.add('edit-btn');
    editBtn.innerHTML = '<i class="fa-solid fa-pen"></i>';
    item.appendChild(editBtn);

    const deleteBtn = document.createElement('button');
    deleteBtn.classList.add('delete-btn');
    deleteBtn.innerHTML = '<i class="fa-solid fa-trash"></i>';
    item.appendChild(deleteBtn);

    itemList.appendChild(item);

    if (below) {
        item.classList.add('below');
    }

    if (save) {
        saveToLocalStorage({name, minimum, quantity, below});
    }

    nameInput.value = '';
    minimumInput.value = '';
    quantityInput.value = '';

    nameInput.focus();
};

const checkBelow = (minimum, quantity) => {
    let below;

    if (quantity < minimum) {
        below = 1;
    } else {
        below = 0;
    }
    
    return below;
};

// Local Storage
const getItemsLocalStorage = () => {
    const items = JSON.parse(localStorage.getItem('items')) || [];
    return items;
};

const saveToLocalStorage = (item) => {
    const items = getItemsLocalStorage();

    items.push(item);
    localStorage.setItem('items', JSON.stringify(items));
};

const deleteFromLocalStorage = (itemName) => {
    const items = getItemsLocalStorage();
    const filteredItems = items.filter((item) => item.name != itemName);
    
    localStorage.setItem('items', JSON.stringify(filteredItems))
};

const loadItems = () => {
    const items = getItemsLocalStorage();

    items.forEach((item) => {
        saveItem(item.name, item.minimum, item.quantity, item.below, 0);
    });
};

// Events
itemForm.addEventListener('submit', (e) => {
    e.preventDefault();

    const nameValue = nameInput.value;
    const minimumValue = minimumInput.value;
    const quantityValue = quantityInput.value;

    if (nameValue && minimumValue && quantityValue) {
        saveItem(nameValue, minimumValue, quantityValue);
    }
});

itemList.addEventListener('click', (e) => {
    targetEl = e.target;
    parentEl = targetEl.closest('div');
    let itemName;
    let itemMinimum;
    let itemQuantity;

    if (parentEl && parentEl.querySelector('.item-info').querySelector('h4')) {
        itemName = parentEl.querySelector('.item-info').querySelector('h4').innerText || '';
        itemMinimum = parentEl.querySelector('.item-info').querySelector('p .min').innerText || '';
        itemQuantity = parentEl.querySelector('.item-info').querySelector('p .quant').innerText || '';
    } 

    if (targetEl.classList.contains('increase-btn')) {
        itemQuantity++;
        let below = checkBelow(itemMinimum, itemQuantity);

        if (below) {
            parentEl.classList.add('below');
        } else {
            parentEl.classList.remove('below');
        }

        parentEl.querySelector('p .quant').innerText = itemQuantity;
    }

    if (targetEl.classList.contains('decrease-btn')) {
        itemQuantity--;
        let below = checkBelow(itemMinimum, itemQuantity);

        if (below) {
            parentEl.classList.add('below');
        } else {
            parentEl.classList.remove('below');
        }

        parentEl.querySelector('p .quant').innerText = itemQuantity;
    }

    if (targetEl.classList.contains('delete-btn')) {
        parentEl.remove();

        deleteFromLocalStorage(itemName);
    }
});

loadItems();