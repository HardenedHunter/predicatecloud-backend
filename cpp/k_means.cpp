#include "k_means.h"
#include <ctime>
#include <algorithm>
#include <utility>
#include <functional>

ivect_t k_means::make_classification(const std::vector<dvect_t>& in_vector, size_t num_dimensions, size_t num_clusters)
{
	const size_t num_vectors = in_vector.size();
	ivect_t result = ivect_t(num_vectors);
	std::vector<dvect_t> centers = init_centers(in_vector, num_vectors, num_dimensions, num_clusters);

	bool is_changed = true;
	while (is_changed)
	{
		result = distribute_vectors(centers, in_vector, num_vectors, num_dimensions, num_clusters, dist);
		is_changed = recalculation_centers(in_vector, num_vectors, num_dimensions, centers, num_clusters, result);
	}
	
	return result;
}

ivect_t k_means::make_classification_pp(const std::vector<dvect_t>& in_vector, size_t num_dimensions,
	size_t num_clusters)
{
	const size_t num_vectors = in_vector.size();
	ivect_t result = ivect_t(num_vectors);
	std::vector<dvect_t> centers = init_centers_pp(in_vector, num_vectors, num_dimensions, num_clusters, square_dist);

	bool is_changed = true;
	while (is_changed)
	{
		result = distribute_vectors(centers, in_vector, num_vectors, num_dimensions, num_clusters, dist);
		is_changed = recalculation_centers(in_vector, num_vectors, num_dimensions, centers, num_clusters, result);
	}

	return result;
}

ivect_t k_means::make_classification_pp_manhattan(const std::vector<dvect_t>& in_vector, size_t num_dimensions,
	size_t num_clusters)
{
	const size_t num_vectors = in_vector.size();
	ivect_t result = ivect_t(num_vectors);
	std::vector<dvect_t> centers = init_centers_pp(in_vector, num_vectors, num_dimensions, num_clusters, square_manhattan_dist);

	bool is_changed = true;
	while (is_changed)
	{
		result = distribute_vectors(centers, in_vector, num_vectors, num_dimensions, num_clusters, manhattan_dist);
		is_changed = recalculation_centers(in_vector, num_vectors, num_dimensions, centers, num_clusters, result);
	}

	return result;
}

std::vector<dvect_t> k_means::init_centers(const std::vector<dvect_t>& in_vector, size_t num_vectors, size_t num_dimensions, size_t num_clusters)
{
	bool is_founded;
	std::vector<dvect_t> centers = std::vector<dvect_t>(num_clusters, dvect_t(num_dimensions, 0));
	ivect_t center_indexes = ivect_t(num_clusters);

	srand(static_cast<unsigned>(time(nullptr)));
	size_t k = rand() % num_vectors;
	for (int i = 0; i < num_clusters; ++i)
	{
		do
		{
			k = rand() % num_vectors;
			is_founded = std::find(center_indexes.begin(), center_indexes.end(), k) != center_indexes.end();
		}
		while (is_founded);

		centers[i] = in_vector[k];
		center_indexes[i] = k;
	}

	return centers;
}

std::vector<dvect_t> k_means::init_centers_pp(const std::vector<dvect_t>& in_vector, size_t num_vectors,
	size_t num_dimensions, size_t num_clusters, std::function<double(dvect_t, dvect_t, size_t)> distance)
{
	std::vector<dvect_t> centers = std::vector<dvect_t>(num_clusters, dvect_t(num_dimensions, 0));
	srand(static_cast<unsigned>(time(nullptr)));
	size_t k = rand() % num_vectors;
	
	k = rand() % num_vectors;
	centers[0] = in_vector[k];

	for (int i = 1; i < num_clusters; ++i)
	{
		dvect_t distances = dvect_t(num_vectors);
		for (int j = 0; j < num_vectors; ++j)
		{
			dvect_t point = in_vector[j];
			double min_dist(std::numeric_limits<double>::infinity());
			double tmp_dist(0);
			
			for (int m = 0; m < i; ++m)
			{
				tmp_dist = distance(point, centers[m], num_dimensions);
				min_dist = std::min(tmp_dist, min_dist);
			}
			distances[j] = min_dist;
		}

		const int vector_max_dist = std::max_element(distances.begin(), distances.end()) - distances.begin();
		centers[i] = in_vector[vector_max_dist];
	}

	return centers;
}


ivect_t k_means::distribute_vectors(const std::vector<dvect_t>& centers, const std::vector<dvect_t>& in_vector, size_t num_vectors, size_t num_dimensions,
	size_t num_clusters, std::function<double(dvect_t, dvect_t, size_t)> distance)
{
	ivect_t result = ivect_t(num_vectors);
	auto it = result.begin();

	std::for_each(in_vector.begin(), in_vector.end(), [&](const dvect_t& in_v)
		{
			dvect_t vector = dist_vector(in_v, centers, num_clusters, num_dimensions, distance);
			const int nearest_center = std::min_element(vector.begin(), vector.end()) - vector.begin();
			*it++ = nearest_center;
		});
	
	return result;
}


double k_means::dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions)
{
	const double distance = square_dist(v1, v2, num_dimensions);
	return sqrt(distance);
}

double k_means::square_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions)
{
	double distance = 0;
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	while (it1 != v1.end())
	{
		distance += pow(*it1++ - *it2++, 2);
	}

	return distance;
}

double k_means::manhattan_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions)
{
	double distance = 0;
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	while (it1 != v1.end())
	{
		distance += abs(*it1++ - *it2++);
	}

	return distance;
}

double k_means::square_manhattan_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions)
{
	return pow(manhattan_dist(v1, v2, num_dimensions), 2);
}

dvect_t k_means::dist_vector(const dvect_t& v, const std::vector<dvect_t>& in_vectors, size_t num_vectors, size_t num_dimensions, std::function<double(dvect_t, dvect_t, size_t)> distance)
{
	dvect_t result = dvect_t(num_vectors);
	auto it_result = result.begin();
	std::for_each(in_vectors.begin(), in_vectors.end(), [&](const dvect_t& in_v) { *it_result++ = distance(v, in_v, num_dimensions); });
	return result;
}

bool k_means::recalculation_centers(const std::vector<dvect_t>& in_vector, size_t num_vectors,
                                    size_t num_dimensions, std::vector<dvect_t>& centers, size_t num_clusters, const ivect_t& center_indexes)
{
	bool is_changed = false;
	for (unsigned int i = 0; i < centers.size(); ++i)
	{
		int count = 0;
		dvect_t new_center = dvect_t(num_dimensions);
		for (int j = 0; j < center_indexes.size(); ++j)
		{
			if (center_indexes[j] == i)
			{
				count++;
				new_center = sum(new_center, in_vector[j]);
			}
		}
		new_center = divide(new_center, count);
		is_changed = new_center != centers[i];
		
		centers[i] = new_center;
	}

	return is_changed;
}

dvect_t k_means::sum(const dvect_t& v1, const dvect_t& v2)
{
	dvect_t sum = v1;
	auto it = sum.begin();
	std::for_each(v2.begin(), v2.end(), [&](const double d) { *it++ += d; });

	return sum;
}

dvect_t k_means::divide(const dvect_t& v, int denominator)
{
	dvect_t result = std::move(v);
	for (auto it = result.begin(); it != result.end(); ++it)
		*it /= denominator;

	return result;
}
