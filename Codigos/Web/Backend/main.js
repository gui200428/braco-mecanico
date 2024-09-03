const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const { SerialPort } = require('serialport');  // Importa SerialPort corretamente
const { ReadlineParser } = require('@serialport/parser-readline');  // Importa ReadlineParser

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
    reconnection: true,
    reconnectionAttempts: Infinity,
    reconnectionDelay: 1000,
    reconnectionDelayMax: 5000,
    timeout: 20000, // 20 segundos
    pingTimeout: 60000 // 60 segundos
});

// Configura a porta serial para comunicação com o Arduino
const port = new SerialPort({ path: 'COM4', baudRate: 9600 });  // Substitua 'COM3' pelo caminho correto da sua porta
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));  // Cria o parser para leitura

// Frontend
app.use(express.static('public'));

// Função para mapear o valor de beta para a faixa de 0 a 180 graus
function mapbetaToServo(beta) {
    // Mapeia -90 (direita) a 90 (esquerda) para 0 a 180 graus
    return Math.round((beta + 90) * (180 / 180));
}

// Socket.io
io.on('connection', (socket) => {
    console.log('Dispositivo conectado!');

    // Receber os dados de orientação do dispositivo
    socket.on('orientationData', (data) => {
        const beta = data.beta;
        const servoPosition = mapbetaToServo(beta);

        console.log(`beta: ${beta}, Servo Position: ${servoPosition}`);

        // Envia o valor mapeado para o Arduino
        port.write(`${servoPosition}\n`);
    });

    // Receber os dados do direcional
    socket.on('crossData', (data) => {
        const direction = data.direction;

        console.log(`Direção recebida: ${direction}`);

        // Envia a direção recebida para o Arduino
        port.write(`${direction}\n`);
    });

    // Receber os dados dos botões X, Y, B, A
    socket.on('buttonData', (data) => {
        const button = data.button.toUpperCase();

        console.log(`Botão recebido: ${button}`);

        // Envia o botão recebido para o Arduino
        port.write(`BUTTON_${button}\n`);
    });

    socket.on('disconnect', () => {
        console.log('Dispositivo desconectado!');
        socket.removeAllListeners();
    });

    // Manter a conexão ativa com ping-pong
    socket.on('ping', () => {
        socket.emit('pong');
    });
});

const PORT = 3000;
server.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
