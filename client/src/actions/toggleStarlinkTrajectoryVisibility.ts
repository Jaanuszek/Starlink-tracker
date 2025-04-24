'use server';

import { fetcher } from '@/lib/fetcher';

export const toggleStarlinkTrajectoryVisibility = async (
	starlinkId: number,
) => {
	const response = await fetcher<{ starlinkId: number }>('ToggleTrajectory', {
		body: {
			starlinkId,
		},
	});

	return response;
};
