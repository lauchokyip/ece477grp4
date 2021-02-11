const express = require("express");
const PORT = process.env.PORT || 3000;
const app = express();
const courses = require("./queue.js");

app.use(express.static("public"));
app.use("/bootstrap", express.static("node_modules/bootstrap/dist/css"));
app.use(express.json({ limit: "1mb" }));
app.set("view engine", "ejs");

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

app.get("/api/courses/:id", (req, res) => {
    res.send(courses.courses);
    // const resCourse = courses.courses(c => c.id === parseInt(req.params.id))
    // res.send(resCourse)
});

app.listen(PORT, () => console.log(`Http listening at port ${PORT}`));
