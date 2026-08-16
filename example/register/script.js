/**
 * SA:MP Mobile CEF - Register Interface JavaScript Logic
 * Handles client-server event communication via Cef.sendEvent and Cef.registerEventCallback
 */

// Fallback mock object if testing directly in a normal Web Browser (outside SA:MP CEF)
if (typeof Cef === 'undefined') {
    window.Cef = {
        sendEvent: function (eventName, dataJson) {
            console.log(`[MOCK CEF OUTGOING] Event: ${eventName}`, JSON.parse(dataJson));
        },
        registerEventCallback: function (eventName, callbackFnName) {
            console.log(`[MOCK CEF LISTENER] Registered callback '${callbackFnName}' for event '${eventName}'`);
        }
    };
}

/**
 * Handle form submission when user clicks "Daftar Sekarang"
 */
function handleFormSubmit(event) {
    event.preventDefault();

    const ucpName = document.getElementById('ucp-name').value.trim();
    const charName = document.getElementById('char-name').value.trim();
    const password = document.getElementById('password').value;
    const confirmPassword = document.getElementById('confirm-password').value;

    // Front-end Validation
    if (password !== confirmPassword) {
        showError('Konfirmasi password tidak cocok dengan password yang dimasukkan!');
        return;
    }

    hideError();
    setLoadingState(true);

    // Prepare JSON Payload for Server
    const payload = [
        ucpName,
        charName,
        password
    ];

    console.log('[CEF Client] Sending registration data to SA:MP Server...');
    
    // Send Event to SA:MP Server (Pawn Script)
    Cef.sendEvent("register_submit", JSON.stringify(payload));
}

/**
 * Handle Discord Quick Login button
 */
function handleDiscordAuth() {
    console.log('[CEF Client] Triggering Discord Authorization...');
    setLoadingState(true);

    // Send Event to SA:MP Server to initiate Discord Auth
    Cef.sendEvent("register_discord", JSON.stringify(["request_auth"]));
}

/**
 * Handle Batal / Cancel button
 */
function handleCancel() {
    console.log('[CEF Client] User cancelled registration.');
    Cef.sendEvent("register_cancel", JSON.stringify(["cancel"]));
}

/**
 * Handle "Masuk ke Dalam Game" button on Success Modal
 */
function handleFinishAndSpawn() {
    console.log('[CEF Client] Finishing registration, requesting player spawn...');
    Cef.sendEvent("register_finish", JSON.stringify(["spawn"]));
}

/**
 * Copy generated UCP PIN to user's clipboard
 */
function copyPinToClipboard() {
    const pinText = document.getElementById('display-pin').innerText;
    navigator.clipboard.writeText(pinText).then(() => {
        alert('PIN UCP berhasil disalin ke clipboard!');
    }).catch(() => {
        alert('PIN: ' + pinText);
    });
}

/**
 * Helper: Show error banner
 */
function showError(message) {
    const banner = document.getElementById('error-banner');
    const msg = document.getElementById('error-message');
    msg.innerText = message;
    banner.classList.remove('hidden');
}

/**
 * Helper: Hide error banner
 */
function hideError() {
    document.getElementById('error-banner').classList.add('hidden');
}

/**
 * Helper: Toggle button loading state
 */
function setLoadingState(isLoading) {
    const btnText = document.getElementById('btn-text');
    const btnSpinner = document.getElementById('btn-spinner');
    const btnSubmit = document.getElementById('btn-submit');

    if (isLoading) {
        btnText.innerText = 'Memproses...';
        btnSpinner.classList.remove('hidden');
        btnSubmit.disabled = true;
    } else {
        btnText.innerText = 'Daftar Sekarang';
        btnSpinner.classList.add('hidden');
        btnSubmit.disabled = false;
    }
}

/**
 * SERVER INCOMING CALLBACK 1: Called from Pawn Server on registration response
 * @param {string} eventDataJson - Serialized JSON array: [statusBool, messageStr, generatedPinStr, ucpNameStr, charNameStr]
 */
function onRegistrationResponse(eventDataJson) {
    setLoadingState(false);
    
    try {
        const data = JSON.parse(eventDataJson);
        const isSuccess = data[0];
        const message = data[1];

        if (!isSuccess) {
            // Show error message sent from Pawn server (e.g. Username already taken)
            showError(message);
            return;
        }

        // Registration Successful! Show UCP PIN Modal
        const pin = data[2] || "123456";
        const ucpName = data[3] || document.getElementById('ucp-name').value;
        const charName = data[4] || document.getElementById('char-name').value;

        document.getElementById('display-pin').innerText = pin;
        document.getElementById('summary-ucp').innerText = ucpName;
        document.getElementById('summary-char').innerText = charName;

        // Hide Register Form, Show Success Card
        document.getElementById('register-card').classList.add('hidden');
        document.getElementById('success-card').classList.remove('hidden');

    } catch (e) {
        console.error('[CEF Client] Error parsing registration response:', e);
        showError('Terjadi kesalahan data dari server!');
    }
}

// Register CEF Listeners (Server -> Web UI)
Cef.registerEventCallback("register_response", "onRegistrationResponse");
