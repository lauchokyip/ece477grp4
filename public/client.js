const publicVapidKey = 'BKUIR2K8S-WoOEqPaluuHgRLf7_ud8OxSMVGiOzUrtBzWdM7mOhDRkwsty5dJsyYcWZtSiVjpsOE1WZofG3v-S0'

  
// Register SW, Register Push, Send Push
async function send() {
  // check if service worker is in the browser first
  if ("serviceWorker" in navigator) {
    // Register Service Worker
    console.log("Registering service worker...");
    
    const register = await navigator.serviceWorker.register("./worker.js", {
      scope: "/"
    });
    console.log("Service Worker Registered...");

    // Register Push
    console.log("Registering Push...");
    const subscription = await register.pushManager.subscribe({
      userVisibleOnly: true,
      applicationServerKey: urlBase64ToUint8Array(publicVapidKey)
    });
    console.log("Push Registered...");

    // Send Push Notification
    console.log("Sending Push...");
    let response = await fetch("/subscribe", {
      method: "POST",
      body: JSON.stringify(subscription),
      headers: {
        "content-type": "application/json"
      }
    });
    if(!response.ok)
    {
      throw new Error(`HTTP Error status: ` + response.status)
    }

    console.log("Push Sent...");
  }
  else {
    alert("This browser does not support service worker. Please install Google Chrome")
  }
}
  
function urlBase64ToUint8Array(base64String) {
  const padding = "=".repeat((4 - base64String.length % 4) % 4);
  const base64 = (base64String + padding)
    .replace(/\-/g, "+")
    .replace(/_/g, "/");

  const rawData = window.atob(base64);
  const outputArray = new Uint8Array(rawData.length);

  for (let i = 0; i < rawData.length; ++i) {
    outputArray[i] = rawData.charCodeAt(i);
  }
  return outputArray;
}