'use server';

import { fetcher } from '@/lib/fetcher';

export const highlightStarlink = async (starlinkId: number) => {
	const response = await fetcher<string>('Highlight', {
		body: {
			starlinkId,
		},
	});

	return response;
};
