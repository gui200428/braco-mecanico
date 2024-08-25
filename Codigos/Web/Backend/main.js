const express = require('express');
const http = require('http');
const socketIo = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

// Frontend
app.use(express.static('public'));

// Socket.io
io.on('connection', (socket) => {
    console.log('Dispositivo conectado!');

    // Receber os dados de orientação do dispositivo
    socket.on('orientationData', (data) => {
        console.log(data);
    });

    socket.on('disconnect', () => {
        console.log('Dispositivo desconectado!');
    });

    // Manter a conexão ativa com ping-pong
    socket.on('ping', () => {
        socket.emit('pong');
    });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
