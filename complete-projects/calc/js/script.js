const previousOperationText = document.querySelector('#previousOperation');
const currentOperationText = document.querySelector('#currentOperation');
const buttons = document.querySelectorAll('#buttonContainer');

class Calculator {
    constructor(previousOperationText, currentOperationText) {
        this.previousOperationText = previousOperationText
        this.currentOperationText = currentOperationText
        this.currentOperation = ''
    }

    // add digit to calculator screen
    addDigit(digit) {
        // check if current operation already has a dot
        if (digit === '.' && this.currentOperationText.innerText.includes('.')) {
            return;
        }

        this.currentOperation = digit;
        this.updateScreen();
    }

    // process all calculator operators
    processOperation(operation) {
        //check if current value is empty
        if (this.currentOperationText.innerText === '' && operation !== 'C') {
            if (this.previousOperationText.innerText !== '') {
                // change operation
                this.changeOperation(operation);
            }
            return;
        }

        // get current and previous value
        let operationValue = '';
        const previous = +this.previousOperationText.innerText.split(' ')[0];
        const current = +this.currentOperationText.innerText;

        switch(operation) {
            case '+':
                operationValue = previous + current
                this.updateScreen(operationValue, operation, previous, current)
                break;
            case '-':
                operationValue = previous - current
                this.updateScreen(operationValue, operation, previous, current)
                break;
            case '/':
                operationValue = previous / current
                this.updateScreen(operationValue, operation, previous, current)
                break;
            case '*':
                operationValue = previous * current
                this.updateScreen(operationValue, operation, previous, current)
                break;
            case 'DEL':
                this.processDelOperator();
                break;
            case 'CE':
                this.processClearCurrentOperation();
                break;
            case 'C':
                this.processClearOperation();
                break;
            case '=':
                this.processEqualOperator();
                break;
            default:
                return;
        }
    }

    // change values of the calculator screen
    updateScreen(operationValue = null, operation = null, previous = null, current = null) {
        if (operationValue === null) {
            this.currentOperationText.innerText += this.currentOperation;
        } else {
            // check if value is zero, if it is just add current value
            if (previous === 0) {
                operationValue = current;
            }
            // add current value to previous
            this.previousOperationText.innerText = `${operationValue} ${operation}`;
            this.currentOperationText.innerText = '';
        }
    }

    changeOperation(operation) {
        const mathOperations = ['+', '-', '*', '/']

        if (!mathOperations.includes(operation)) {
            return;
        }
        this.previousOperationText.innerText = this.previousOperationText.innerText.slice(0, -1) + operation;
    }

    processDelOperator() {
        this.currentOperationText.innerText = this.currentOperationText.innerText.slice(0, -1);
    }

    processClearCurrentOperation() {
        this.currentOperationText.innerText = '';
    }

    processClearOperation() {
        this.previousOperationText.innerText = '';
        this.currentOperationText.innerText = '';
    }

    processEqualOperator() {
        const operation = previousOperationText.innerText.split(' ')[1];

        this.processOperation(operation);
        this.currentOperationText.innerText = this.previousOperationText.innerText.split(' ')[0];
    }
};

const calc = new Calculator(previousOperationText, currentOperationText);

buttons.forEach((btn) => {
    btn.addEventListener('click', (e) => {
        const btnValue = e.target.innerText

        if (+btnValue >= 0 || btnValue === '.') {
            calc.addDigit(btnValue);
        } else {
            calc.processOperation(btnValue);
        }
    });
});