'use server';

import { fetcher } from '@/lib/fetcher';

export const unhighlightStarlink = async () => {
	const response = await fetcher<string>('Unhighlight', {
		method: 'POST',
	});

	return response;
};
