const {
  k_means,
  k_means_pp,
  k_means_pp_manhattan,
} = require('./build/Release/addon.node');
console.log(
  k_means_pp(
    [
      [1, 2],
      [3, 4],
      [5, 6],
      [7, 8],
    ],
    2
  )
);
