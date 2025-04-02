'use client';

import { QueryClientProvider } from '@tanstack/react-query';
import { ThemeProvider } from 'next-themes';
import { ReactNode } from 'react';

import { getQueryClient } from '@/lib/queryClient';

export default function Providers({ children }: { children: ReactNode }) {
	const queryClient = getQueryClient();

	return (
		<QueryClientProvider client={queryClient}>
			<ThemeProvider
				enableSystem
				disableTransitionOnChange
				attribute='class'
				defaultTheme='dark'
			>
				{children}
			</ThemeProvider>
		</QueryClientProvider>
	);
}
