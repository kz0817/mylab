'use strict'
const express = require('express')
const router = express.Router()

router.get('/', function(req, res) {
  res.send('page1')
})

module.exports = router
