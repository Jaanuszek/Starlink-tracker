import { useForm } from 'react-hook-form';
import { zodResolver } from '@hookform/resolvers/zod';

import { useHighlightStarlink } from '@/mutations/useHighlightStarlink';
import {
	HighlightStarlinkData,
	highlightStartlinkSchema,
} from '@/schemas/highlightStarlinkSchema';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const HighlighteStarlinkTab = () => {
	const form = useForm({
		resolver: zodResolver(highlightStartlinkSchema),
		defaultValues: { starlinkId: '' },
	});

	const { mutateAsync: enableStarlink, isPending } = useHighlightStarlink();

	const handleSubmit = async (data: HighlightStarlinkData) => {
		await enableStarlink(data.starlinkId);
	};

	return (
		<TabsContent value='highlight'>
			<Card>
				<CardContent>
					<Form {...form}>
						<form onSubmit={form.handleSubmit(handleSubmit)}>
							<FormField
								control={form.control}
								name='starlinkId'
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
								Highlight starlink
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
