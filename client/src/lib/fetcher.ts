type FetcherBody = BodyInit | Record<string, unknown> | unknown[] | null;

type FetcherOptions = Omit<RequestInit, 'body'> & {
	body?: FetcherBody;
};

export const fetcher = async <T>(
	endpoint: string,
	options: FetcherOptions = {},
): Promise<T> => {
	const { body, headers, ...rest } = options;
	const headersObject = {
		'Content-Type': 'application/json',
		...headers,
	};

	const method = body ? 'POST' : 'GET';

	const config: RequestInit = {
		method,
		credentials: 'include',
		headers: headersObject,
		...rest,
	};

	if (body) {
		config.body = JSON.stringify(body);
	}

	const response = await fetch(`http://localhost:8080/${endpoint}`, config);
	const data = await response.json();

	return data as T;
};
