// Variáveis e seleção de elementos
const apiKey = 'c023cadecc58b84dcd7327372b3ebf9a';
const apiUnsplash = "https://source.unsplash.com/1600x900/?";

const cityInput = document.querySelector('#city-input');
const searchBtn = document.querySelector('#search');
const form = document.querySelector('.form');

const cityElement = document.querySelector('#city');
const tempElement = document.querySelector('#temperature span');
const descElement = document.querySelector('#description');
const weathericonElement = document.querySelector('#weather-icon');
const countryElement = document.querySelector('#country');
const humidityElement = document.querySelector('#humidity span');
const windElement = document.querySelector('#wind span');

const weatherData = document.querySelector('#weather-data');
const loader = document.querySelector('#loader');
const errorMessage = document.querySelector('#error-message');

// Funções
const hideInfo = () => {
    weatherData.classList.add('hide');
    errorMessage.classList.add('hide');
};

const toggleLoader = () => {
    loader.classList.toggle('hide');
};

const showErrorMessage = () => {
    errorMessage.classList.remove('hide')
};

const getWeatherData = async (city) => {
    toggleLoader();

    const apiWeatherURL = `https://api.openweathermap.org/data/2.5/weather?q=${city}&units=metric&appid=${apiKey}&lang=pt_br`;

    const res = await fetch(apiWeatherURL);
    const data = await res.json();

    toggleLoader();

    return data;
};

const showWeatherData = async(city) => {
    hideInfo();

    const data  = await getWeatherData(city);

    if (data.cod === '404') {
        showErrorMessage();
        return;
    }

    cityElement.innerText = data.name;
    tempElement.innerText = parseInt(data.main.temp);
    descElement.innerText = data.weather[0].description;
    weathericonElement.setAttribute('src', `http://openweathermap.org/img/wn/${data.weather[0].icon}.png`);
    countryElement.setAttribute('src', `https://flagsapi.com/${data.sys.country}/flat/64.png`);
    humidityElement.innerText = `${data.main.humidity}%`;
    windElement.innerText = `${data.wind.speed}km/h`;

    document.body.style.backgroundImage = `url("${apiUnsplash + city}")`;

    weatherData.classList.remove('hide');
};

// Eventos
form.addEventListener('submit', (e) => {
    e.preventDefault();

    const city = cityInput.value;
    
    showWeatherData(city);
    cityInput.value = '';
    cityInput.focus();
});