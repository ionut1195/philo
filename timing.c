#include <sys/time.h>

long long unsigned int	get_current_timestamp(void)
{
	struct timeval			current_time;
	long long unsigned int	timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	return (timestamp);
}
