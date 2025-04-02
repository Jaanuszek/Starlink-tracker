import { useForm } from 'react-hook-form';
import { zodResolver } from '@hookform/resolvers/zod';

import { useEnableStarlink } from '@/mutations/useEnableStarlink';
import {
	EnableStarlinkData,
	enableStartlinkSchema,
} from '@/schemas/enableStarlinkSchema';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const EnableStarlinkTab = () => {
	const form = useForm({
		resolver: zodResolver(enableStartlinkSchema),
		defaultValues: { starlinkId: '' },
	});

	const { mutateAsync: enableStarlink, isPending } = useEnableStarlink();

	const handleSubmit = async (data: EnableStarlinkData) => {
		await enableStarlink(data.starlinkId);
	};

	return (
		<TabsContent value='enable'>
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
								Show Starlink
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
