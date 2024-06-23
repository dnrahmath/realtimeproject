const chatDiv = document.getElementById('chat');
const messageInput = document.getElementById('message');
const ws = new WebSocket('ws://localhost:8080');

let room = ''; // Variabel untuk menyimpan nama room chat

ws.onopen = () => {
    chatDiv.innerHTML += '<div>Connected to the server.</div>';
    // Setelah terhubung, kita minta pengguna untuk memasukkan nama room chat
    room = prompt('Enter chat room name:');
    if (room) {
        // Kirim pesan ke server bahwa kita ingin bergabung dengan room tertentu
        ws.send(JSON.stringify({ type: 'join', room: room }));
    } else {
        alert('Room name cannot be empty. Please refresh and try again.');
    }
};

ws.onmessage = (event) => {
    const message = JSON.parse(event.data);
    if (message.type === 'message') {
        chatDiv.innerHTML += '<div><b>' + message.sender + ':</b> ' + message.text + '</div>';
        chatDiv.scrollTop = chatDiv.scrollHeight;
    }
};

ws.onclose = () => {
    chatDiv.innerHTML += '<div>Disconnected from the server.</div>';
};

function sendMessage() {
    const message = messageInput.value;
    if (message) {
        // Kirim pesan ke server dengan menyertakan room
        ws.send(JSON.stringify({ type: 'message', room: room, text: message }));
        messageInput.value = '';
    }
}

messageInput.addEventListener('keypress', (event) => {
    if (event.key === 'Enter') {
        sendMessage();
    }
});
