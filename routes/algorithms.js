const express = require('express');
const { kmeansSchema, validateAlgorithm } = require('./../models/algorithm');
const router = express.Router();
const auth = require('../middleware/authentication');
const {
  k_means,
  k_means_pp,
  k_means_pp_manhattan,
} = require('../build/Release/addon.node');

const algorithms = [k_means, k_means_pp, k_means_pp_manhattan];

router.post('/', [auth], async (req, res) => {
  const { error } = validateAlgorithm(req.body, kmeansSchema);
  if (error) return res.status(400).send(error.details[0].message);

  const { algorithm, data } = req.body;
  const { clusters, values } = data;

  let result = [];
  try {
    result = algorithms[algorithm](values, clusters);
  } catch (error) {
    return res.status(400).send('Something bad happened.');
  }

  res.send({ result });
});

module.exports = router;
