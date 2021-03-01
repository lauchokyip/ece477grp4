const webpush = require("web-push")
const publicVapidKey =  "BKUIR2K8S-WoOEqPaluuHgRLf7_ud8OxSMVGiOzUrtBzWdM7mOhDRkwsty5dJsyYcWZtSiVjpsOE1WZofG3v-S0";
const privateVapidKey = "qW6a1325o4JjBqUc7ylFLeL88Y_4Fgp28sUBCNLOYZI";

let db = [];

webpush.setVapidDetails(
  "mailto:test@test.com",
  publicVapidKey,
  privateVapidKey
);


function subscriptionObjectIndexOnDB(customerID){
  for(let i = 0; i < db.length; i++){
    if(db[i].id === customerID)
      return i;
  }
  return -1;
}

/**
 * isValidSaveRequest will validate if the request is valid or not
 * @param {} req
 * @param {*} res
 */
const isValidSaveRequest = (req, res) => {
  // Check the request body has at least an endpoint.
  if (!req.body || !req.body.subscription.endpoint) {
      // Not a valid subscription.
      res.status(400);
      res.setHeader("Content-Type", "application/json");
      res.send(
          JSON.stringify({
              error: {
                  id: "no-endpoint",
                  message: "Subscription must have an endpoint.",
              },
          })
      );
      return false;
  }
  return true;
};

/**
 * contains will check if the Subscription object in the array has the same endpoint
 * return true if the endpoint already exists otherwise false
 * @param {*} a 
 * @param {*} obj 
 */
function contains(a, obj) {
  var i = a.length;
  while (i--) {
     if (a[i]["subscription"]["endpoint"] === obj["subscription"]["endpoint"]) {
         return true;
     }
  }
  return false;
}

/**
 * saveSubscriptionToDatabase will save user Subsription in array
 * @param {*} subscription
 */
function saveSubscriptionToDatabase(idAndSubscriptionObj) {
  return new Promise((resolve, reject) => {
    console.log(idAndSubscriptionObj)
      if(!contains(db, idAndSubscriptionObj))
        db.push(idAndSubscriptionObj)
      resolve(true);
  });
}

/**
 * getSubscriptionsFromDatabase will return the whole array containing the Subscription object
 * @param {*} db 
 */
function getSubscriptionsFromDatabase() {
  return new Promise((resolve, reject) => {
      resolve(db);
  });
};


function clearAllSubscriptionsFromDatabaes() {
  return new Promise((resolve, reject) => {
    db = [];
    resolve(db);
});
} 

/**
 * getSubscriptionsByID will return the Subscription Object based on the customerID
 * return array Index if true
 * otherwise return -1
 * @param {*} customerId 
 */
function getSubscriptionsByID(customerId) {
  return new Promise((resolve, reject) => { 
    const ret = subscriptionObjectIndexOnDB(customerId)
    if(ret !== -1)
      resolve(db[ret]["subscription"])
    else
      reject("Subscription object is not in the array")
  });
}

/**
 * delelteSubsctiptionByID will return true if the Subscription object is successfully deleted
 * otherwise return false
 */
function deleteSubsctiptionByID(customerId) {
    const ret = subscriptionObjectIndexOnDB(customerId)
    if(ret !== -1)
    {
      deletedObject = queue.splice(ret, 1);
      if (deletedObject == undefined) 
        return false;
      return true;
    }
    return false;
}

/**
 * triggerPushMsg will push message once it's called
 * @param {*} subscription
 * @param {*} dataToSend
 */
function triggerPushMsg(subscriptionObj, dataToSend) {
  return webpush.sendNotification(subscriptionObj, dataToSend)
 
};

function triggerPushMsgByID(customerID)
{
  console.log("Notifying " + customerID)
  return getSubscriptionsByID(customerID)
  .then((subscriptionObj) => {
      triggerPushMsg(subscriptionObj)
  }).then(() => {
      console.log(`Data successfully sent to '${customerId}'`)
      return true
  }).catch((err) => {
      return err
  })    
}

module.exports = {
  isValidSaveRequest,
  saveSubscriptionToDatabase,
  getSubscriptionsFromDatabase,
  deleteSubsctiptionByID,
  triggerPushMsg,
  triggerPushMsgByID,
  clearAllSubscriptionsFromDatabaes,
}