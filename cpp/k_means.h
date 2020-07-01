#pragma once
#include <vector>
#include <functional>

typedef std::vector<double> dvect_t;
typedef std::vector<size_t> ivect_t;

class k_means
{
public:
	// k-means
	static ivect_t make_classification(const std::vector<dvect_t>& in_vector, size_t num_dimensions, size_t num_clusters);

	// k-means++
	static ivect_t make_classification_pp(const std::vector<dvect_t>& in_vector, size_t num_dimensions, size_t num_clusters);

	//  k-means++ with manhattan
	static ivect_t make_classification_pp_manhattan(const std::vector<dvect_t>& in_vector, size_t num_dimensions, size_t num_clusters);

	static double dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions);
	static double square_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions);
	static double manhattan_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions);
	static double square_manhattan_dist(const dvect_t& v1, const dvect_t& v2, size_t num_dimensions);
	
private:
	static std::vector<dvect_t> init_centers(const std::vector<dvect_t>& in_vector, size_t num_vectors, size_t num_dimensions, size_t num_clusters);
	static std::vector<dvect_t> init_centers_pp(const std::vector<dvect_t>& in_vector, size_t num_vectors, size_t num_dimensions, size_t num_clusters, std::function<double(dvect_t, dvect_t, size_t)> distance = square_dist);
	static ivect_t distribute_vectors(const std::vector<dvect_t>& centers, const std::vector<dvect_t>& in_vector, size_t num_vectors, size_t num_dimensions, size_t num_clusters, std::function<double(dvect_t, dvect_t, size_t)> distance = dist);

	static dvect_t dist_vector(const dvect_t& v, const std::vector<dvect_t>& in_vectors, size_t num_vectors, size_t num_dimensions, std::function<double(dvect_t, dvect_t, size_t)> distance = dist);
	static bool recalculation_centers(const std::vector<dvect_t>& in_vector, size_t num_vectors,
	                                  size_t num_dimensions, std::vector<dvect_t>& centers, size_t num_clusters, const ivect_t& center_indexes);
	static dvect_t sum(const dvect_t& v1, const dvect_t& v2);
	static dvect_t divide(const dvect_t& v, int denominator);
};



