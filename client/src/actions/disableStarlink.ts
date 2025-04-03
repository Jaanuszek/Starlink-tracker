'use server';

import { fetcher } from '@/lib/fetcher';

export const disableStarlink = async (starlinkId: string) => {
	const response = await fetcher<string>('DisableStarlink', {
		body: {
			starlinkId: parseInt(starlinkId.trim(), 10),
		},
	});

	return response;
};
