import { zodResolver } from '@hookform/resolvers/zod';
import { useForm } from 'react-hook-form';

import { useLoadStarlinks } from '@/mutations/useLoadStarlinks';
import {
	LoadStarlinkData,
	loadStarlinkSchema,
} from '@/schemas/loadStarlinksSchema';
import { Button } from './ui/button';
import { Card, CardContent } from './ui/card';
import { Form, FormField, FormItem, FormControl, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { TabsContent } from './ui/tabs';

export const LoadStarlinksTab = () => {
	const form = useForm({
		resolver: zodResolver(loadStarlinkSchema),
		defaultValues: { starlinkIds: '' },
	});

	const { mutateAsync: loadStarlinks, isPending } = useLoadStarlinks();

	const handleSubmit = async (data: LoadStarlinkData) => {
		await loadStarlinks(data.starlinkIds);
	};

	return (
		<TabsContent value='load'>
			<Card>
				<CardContent>
					<Form {...form}>
						<form onSubmit={form.handleSubmit(handleSubmit)}>
							<FormField
								control={form.control}
								name='starlinkIds'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input {...field} placeholder='Enter ID, f.e. 123' />
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<Button disabled={isPending} type='submit' className='mt-2'>
								Load starlinks
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
