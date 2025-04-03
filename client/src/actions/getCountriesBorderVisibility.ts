'use server';

import { fetcher } from '@/lib/fetcher';

type GetCountriesBorderVisibilityResponse = {
	isCountriesBorderVisible: boolean;
};

export const getCountriesBorderVisibility = async () => {
	const response = await fetcher<GetCountriesBorderVisibilityResponse>(
		'GetCountriesBorderVisibility',
	);

	return response;
};
