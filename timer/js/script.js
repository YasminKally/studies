const minutesEl = document.querySelector('#minutes');
const secondsEl = document.querySelector('#seconds');
const milisecondsEl = document.querySelector('#miliseconds');
const startBtn = document.querySelector('#start-btn');
const pauseBtn = document.querySelector('#pause-btn');
const resumeBtn = document.querySelector('#resume-btn');
const resetBtn = document.querySelector('#reset-btn');

let interval;
let minutes = 0;
let seconds = 0;
let miliseconds = 0;
let isPaused = false;

// Functions 
const startTimer = () => {
    interval = setInterval(() => {
        if (!isPaused) {
            miliseconds += 10;

            if (miliseconds === 1000) {
                miliseconds = 0;
                seconds++;
            }

            if (seconds === 60) {
                seconds = 0;
                minutes++;
            }

            minutesEl.textContent = formatTime(minutes);
            secondsEl.textContent = formatTime(seconds);
            milisecondsEl.textContent = formatMiliseconds(miliseconds);
        }
    }, 10)
    startBtn.style.display = 'none';
    pauseBtn.style.display = 'block';
    resetBtn.style.display = 'block';
};

const formatTime = (time) => {
    return time < 10 ? `0${time}` : time;
};

const formatMiliseconds = (time) => {
    return time < 100 ? `${time}`.padStart(3, '0') : time
};

// Events
startBtn.addEventListener('click', startTimer);

pauseBtn.addEventListener('click', () => {
    isPaused = true;
    pauseBtn.style.display = 'none'
    resumeBtn.style.display = 'block'
});

resumeBtn.addEventListener('click', () => {
    isPaused = false;
    resumeBtn.style.display = 'none';
    pauseBtn.style.display = 'block';
});

resetBtn.addEventListener('click', () => {
    clearInterval(interval);
    minutes = 0;
    seconds = 0;
    miliseconds = 0;

    minutesEl.textContent = formatTime(minutes);
    secondsEl.textContent = formatTime(seconds);
    milisecondsEl.textContent = formatMiliseconds(miliseconds);
    resetBtn.style.display = 'none';
    pauseBtn.style.display = 'none';
    resetBtn.style.display = 'none';
    startBtn.style.display = 'block';
});