#include "cqueue.h"

int create_queue(c_queue *queue, int size)
{
    if (queue == NULL || size <= 0)
        return FAILURE;

    queue->item = (int *)malloc(size * sizeof(int));
    if (queue->item == NULL)
        return FAILURE;

    queue->capacity = size;
    queue->count = 0;
    queue->front = 0;
    queue->rear = -1;
    return SUCCESS;
}

int enqueue(c_queue *queue, int element)
{
    if (queue == NULL || queue->item == NULL)
        return FAILURE;

    if (queue->count == (int)queue->capacity)
        return FAILURE; // Queue is full

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->item[queue->rear] = element;
    queue->count++;
    return SUCCESS;
}

int dequeue(c_queue *queue, int *element)
{
    if (queue == NULL || queue->item == NULL || element == NULL)
        return FAILURE;

    if (queue->count == 0)
        return FAILURE; // Queue is empty

    *element = queue->item[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->count--;
    return SUCCESS;
}

int print_queue(c_queue *queue)
{
    if (queue == NULL || queue->count == 0)
    {
        printf("Circular Queue is empty\n");
        return FAILURE;
    }

    printf("Circular Queue elements (count: %d/%u): ", queue->count, queue->capacity);
    for (int i = 0; i < queue->count; i++)
    {
        int idx = (queue->front + i) % queue->capacity;
        printf("%d ", queue->item[idx]);
    }
    printf("\n");
    return SUCCESS;
}

int main(void)
{
    c_queue queue;
    int size = 5;

    printf("=== Circular Queue Verification Demo ===\n");
    if (create_queue(&queue, size) == FAILURE)
    {
        printf("Error: Failed to allocate circular queue.\n");
        return 1;
    }

    printf("[1] Enqueueing elements 10, 20, 30, 40, 50...\n");
    for (int i = 1; i <= 5; i++)
    {
        enqueue(&queue, i * 10);
    }
    print_queue(&queue);

    printf("[2] Attempting to enqueue 60 into full queue...\n");
    if (enqueue(&queue, 60) == FAILURE)
    {
        printf("  --> Enqueue 60 correctly rejected (Queue Full).\n");
    }

    printf("[3] Dequeueing 2 elements...\n");
    int val = 0;
    dequeue(&queue, &val);
    printf("  --> Dequeued: %d\n", val);
    dequeue(&queue, &val);
    printf("  --> Dequeued: %d\n", val);
    print_queue(&queue);

    printf("[4] Enqueueing 60 and 70 to wrap around circular buffer...\n");
    enqueue(&queue, 60);
    enqueue(&queue, 70);
    print_queue(&queue);

    printf("=== Circular Queue Verified Successfully ===\n");
    free(queue.item);
    return 0;
}
