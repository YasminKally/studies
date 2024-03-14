const qst = document.querySelector('.question');
const answers = document.querySelector('.answers');
const spnQtd = document.querySelector('.spnQtd');
const textFinish = document.querySelector('.finish span');
const content = document.querySelector('.content');
const contentFinish = document.querySelector('.finish');
const restartBtn = document.querySelector('.finish button');

const questions = [
    {
        question: 'Qual o nome da minha gata?',
        answers: [
            {option: "Nix", correct: false},
            {option: "Nyx", correct: true},
            {option: "Ninx", correct: false},
        ],
    },
    {
        question: 'Meu jogo favorito?',
        answers: [
            {option: "Monster Hunter", correct: false},
            {option: "Street Fighter", correct: false},
            {option: "Nenhum em específico", correct: true},
        ],
    },
    {
        question: 'O que eu mais gosto de fazer?',
        answers: [
            {option: "Dormir", correct: false},
            {option: "Jogar", correct: false},
            {option: "Andar de bicicleta", correct: true},
        ],
    },
    {
        question: 'Minha idade no final desse ano?',
        answers: [
            {option: "19", correct: true},
            {option: "20", correct: false},
            {option: "21", correct: false},
        ],
    },
];

let currentIndex = 0;
let correctQuestions = 0;

restartBtn.onclick = () => {
    content.style.display = 'flex';
    contentFinish.style.display = 'none';
    currentIndex = 0;
    correctQuestions = 0;
    loadQuestion();
};

const nextQuestion = (e) => {
    if(e.target.getAttribute('data-correct') === 'true') {
        correctQuestions++;
    }

    if (currentIndex < questions.length - 1) {
        currentIndex++;
        loadQuestion();
    } else {
        finish();
    }
};

const finish = () => {
    textFinish.innerHTML = `Você acertou ${correctQuestions} de ${questions.length}`;
    content.style.display = 'none';
    contentFinish.style.display = 'flex';
};

const loadQuestion = () => {
    spnQtd.innerHTML = `${currentIndex + 1}/${questions.length}`;
    const item = questions[currentIndex];
    answers.innerHTML = "";
    qst.innerHTML = item.question;

    item.answers.forEach((answer) => {
        const div = document.createElement('div');

        div.innerHTML = `
        <button class="answer" data-correct="${answer.correct}">
            ${answer.option}
        </button>
        `;

        answers.appendChild(div);
    });

    document.querySelectorAll('.answer').forEach((item) => {
        item.addEventListener('click', nextQuestion);
    });
};

loadQuestion();