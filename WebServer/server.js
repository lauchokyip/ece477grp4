const express = require("express");

const PORT = process.env.PORT || 3000;

const app = express();
app.use(express.static("public"));
app.use(express.json({ limit: "1mb" }));
app.set("view engine", "ejs");

var database = {};
// datbase {
//     queue:[
//         {
//             name:
//             id:
//             #peopleInParty:
//             email:
//         },
//         {},

//     ],
//     numPeople:,

// }
app.get("/", (req, res) => {
    res.render("index");
});

app.get("/homepage", (req, res) => {
    res.render("homepage");
});

app.listen(PORT, () => console.log("Http listening at port %d", PORT));
