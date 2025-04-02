'use server';

import { fetcher } from '@/lib/fetcher';

export const toggleCountriesBorderVisibility = async () => {
	const response = await fetcher<{
		isCountriesBorderVisible: boolean;
		message: string;
	}>('ToggleCountriesBorder', {
		method: 'POST',
	});

	return response;
};
