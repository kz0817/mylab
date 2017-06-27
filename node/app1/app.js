var express = require("express");
var app = express();

app.get("/", function(req, res, next){
    res.json({"name": "dog"});
});

var server = app.listen(process.env.PORT || 3000, function(){
    console.log("Node.js is listening to PORT:" + server.address().port);
});

