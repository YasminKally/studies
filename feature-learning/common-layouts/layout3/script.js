openMenu.addEventListener('click', () => {
    openMenu.style.display = 'none'
    menu.style.display = 'flex'
    menu.style.right = (menu.offsetWidth * -1) + 'px'
    setTimeout(() => {
        menu.style.right = '0'
        menu.style.opacity = '1'
    }, 10)
})

closeMenu.addEventListener('click' , () => {
    menu.style.opacity = '0'
    menu.style.right = (menu.offsetWidth * -1) + 'px'
    setTimeout(() => {
        menu.removeAttribute('style')
        openMenu.removeAttribute('style')
    }, 200)
})