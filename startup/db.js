const mongoose = require('mongoose');
const config = require('config');
const { User } = require('../models/user');

module.exports = function () {
  const db = config.get('db');
  mongoose
    .connect(db, { useNewUrlParser: true, useUnifiedTopology: true })
    .then(() => console.log('Connected to database'))
    .then(async () => {
      if (process.env.NODE_ENV !== 'production') await User.deleteMany();
    })
    .catch(() => console.error('Could not connect to MongoDB'));
};
