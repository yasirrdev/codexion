#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (parse_args(&data, argc, argv))
		return (1);
	if (init_data(&data))
		return (1);

	assign_dongles(&data);

	start_threads(&data);

	return (0);
}
