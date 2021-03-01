const publicVapidKey = 'BKUIR2K8S-WoOEqPaluuHgRLf7_ud8OxSMVGiOzUrtBzWdM7mOhDRkwsty5dJsyYcWZtSiVjpsOE1WZofG3v-S0'
// TODO: force people to use Chrome
var isChromium = window.chrome;
var winNav = window.navigator;
var vendorName = winNav.vendor;
var isOpera = typeof window.opr !== "undefined";
var isIEedge = winNav.userAgent.indexOf("Edge") > -1;
var isIOSChrome = winNav.userAgent.match("CriOS");

if (isIOSChrome) {
   // is Google Chrome on IOS
   console.log("Is Google Chrome")
} else if(
  isChromium !== null &&
  typeof isChromium !== "undefined" &&
  vendorName === "Google Inc." &&
  isOpera === false &&
  isIEedge === false
) {
   // is Google Chrome
   console.log("Is Google Chrome")
} else { 
   // not Google Chrome 
   // TODO: tell user to use Google Chrome
   console.log("Is Not Google Chrome")

}

export async function registerSW(customerId) {
    if (window.Notification) {
      Notification.requestPermission( async (status) => {
        console.log('Status of the request:', status)
        if (Notification.permission !== 'granted') {
          // TODO: Display that we need notification in order for it to work
        } else {
              // check if service worker is in the browser first
          if ("serviceWorker" in navigator) {
            try {
              // Register Service Worker
              console.log("Registering service worker...");
              
              const registerSW1 = await navigator.serviceWorker.register("./worker.js", {
                scope: "/"
              });
              console.log("Service Worker Registered...");
          
              // Register Push
              console.log("Registering Push...");
              const subscription = await registerSW1.pushManager.subscribe({
                userVisibleOnly: true,
                applicationServerKey: urlBase64ToUint8Array(publicVapidKey)
              });
              console.log("Push Registered...");
          
              // Send Push Notification
              console.log("Sending Push...");
              const idAndSubsciption = {
                id: customerId,
                subscription: subscription
              }
              console.log(idAndSubsciption)
              let response = await fetch("/subscribe", {
                method: "POST",
                body: JSON.stringify(idAndSubsciption),
                headers: {
                  "content-type": "application/json"
                }
              });
              if(!response.ok)
              {
                throw new Error(`HTTP Error status: ` + response.status)
              } 
          
              console.log("Push Sent...");
            } catch(error) {
              console.log(error)
              return error
            }
          }
          else {
            alert("This browser does not support service worker. Please install Google Chrome")
          }
        }
      })
      
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