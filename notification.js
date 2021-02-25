
let db = [];

/**
 * isValidSaveRequest will validate if the request is valid or not
 * @param {} req
 * @param {*} res
 */
const isValidSaveRequest = (req, res) => {
  // Check the request body has at least an endpoint.
  if (!req.body || !req.body.endpoint) {
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
     if (a[i]["endpoint"] === obj["endpoint"]) {
         return true;
     }
  }
  return false;
}

/**
 * saveSubscriptionToDatabase will save user Subsription in array
 * @param {*} subscription
 */
function saveSubscriptionToDatabase(subscription) {
  return new Promise((resolve, reject) => {
      if(!contains(db, subscription))
       db.push(subscription); 
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

module.exports = {
  isValidSaveRequest,
  saveSubscriptionToDatabase,
  getSubscriptionsFromDatabase,
  clearAllSubscriptionsFromDatabaes,
}