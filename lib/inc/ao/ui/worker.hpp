#include <future>
#include <Eigen/Dense>

#include "ao/gl/core.hpp"
#include "ao/render/heightmap.hpp"

/*
 *  A worker contains all of the data needed for a running render task
 */
struct Worker
{
    /*
     *  Constructs a region from the given voxel count and a divisor
     *  (higher divisors produce lower-resolution workers)
     */
    Worker(Tree* tree, size_t ni, size_t nj, size_t nk, double div);

    /*
     *  On destruction, join the thread
     */
    ~Worker();

    /*
     *  Returns true if the worker is running, false otherwise
     */
    bool running() const;

    /*
     *  Attempts to halt the task by setting the abort flag
     */
    void halt();

    /*
     *  Polls the worker, loading data into the given textures if complete
     *  Returns true if the worker is done running, false otherwise
     */
    bool poll(GLuint depth, GLuint norm);

    /*  Region that is being analyzed  */
    Region region;

    std::promise<std::pair<Eigen::ArrayXXd, Image>> promise;
    std::future<std::pair<Eigen::ArrayXXd, Image>> future;
    std::atomic<bool> abort;
    std::thread thread;
};