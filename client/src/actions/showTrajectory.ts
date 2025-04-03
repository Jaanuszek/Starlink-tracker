'use server';

import { fetcher } from '@/lib/fetcher';

export const showTrajectory = async (starlinkId: string) => {
	const response = await fetcher<string>('ShowTrajectory', {
		body: {
			starlinkId: parseInt(starlinkId.trim(), 10),
		},
	});

	return response;
};
