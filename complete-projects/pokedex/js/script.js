const pokemonImage = document.querySelector('.pokemon-image');
const pokeNumber = document.querySelector('.poke-number');
const pokeName = document.querySelector('.poke-name');
const form = document.querySelector('.form');
const search = document.querySelector('.search');
const prevBtn = document.querySelector('.btn-prev');
const nextBtn = document.querySelector('.btn-next');

let searchPokemon = 1;

// Functions
const fetchPokemon = async (pokemon) => {
    const APIresponse = await fetch(`https://pokeapi.co/api/v2/pokemon/${pokemon}`);

    if (APIresponse.status == '200') {
        const data = await APIresponse.json();
        return data;
    }
};

const renderPokemon = async (pokemon) => {
    pokeName.innerText = 'Loading...'
    pokeNumber.innerHTML = '';

    const data = await fetchPokemon(pokemon);

    if (data) {
        searchPokemon = data.id;

        pokeName.innerText = data.name;
        pokeNumber.innerText = data.id;
        pokemonImage.style.display = 'block'
        pokemonImage.setAttribute('src', data['sprites']['versions']['generation-v']['black-white']['animated']['front_default']);
    } else {
        pokeName.innerText = 'Not found';
        pokeNumber.innerHTML = '';
        pokemonImage.style.display = 'none'
    }
};

// Events
form.addEventListener('submit', (e) => {
    e.preventDefault();

    renderPokemon(search.value.toLowerCase());
    search.value = '';
    search.focus();
});

prevBtn.addEventListener('click', () => {
    if (searchPokemon > 1) {
            searchPokemon--;
        renderPokemon(searchPokemon);
    }
});

nextBtn.addEventListener('click', () => {
    searchPokemon++;
    renderPokemon(searchPokemon); 
});

// Init app
renderPokemon(searchPokemon);