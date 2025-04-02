'use server';

import { fetcher } from '@/lib/fetcher';

export const loadStarlinks = async (starlinkIds: string) => {
	const response = await fetcher<string>('LoadStarlinks', {
		body: {
			starlinkIds: starlinkIds.split(',').map((id) => parseInt(id.trim(), 10)),
		},
	});

	return response;
};
