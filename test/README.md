# Unit Tests

This directory contains unit tests for the Battery Assembly Testing System.

## Running Tests

### Using PlatformIO CLI
```bash
# Run all tests
pio test

# Run tests for specific environment
pio test -e esp32dev

# Run specific test
pio test -f test_battery_measurements

# Verbose output
pio test -v
```

### Using VS Code
1. Open PlatformIO sidebar
2. Navigate to "Test" section
3. Select environment and click "Test" button

## Creating Tests

Create test files in this directory with the naming convention `test_*.cpp` or `test_*.c`.

### Example Test Structure

```cpp
#include <unity.h>

// Test functions
void test_voltage_measurement(void) {
    // Arrange
    float expected = 12.0;
    
    // Act
    float actual = measureVoltage();
    
    // Assert
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected, actual);
}

void setUp(void) {
    // Set up test environment
}

void tearDown(void) {
    // Clean up after test
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_voltage_measurement);
    
    return UNITY_END();
}
```

## Test Framework

PlatformIO uses the [Unity Test Framework](http://www.throwtheswitch.org/unity) by default.

### Common Assertions

- `TEST_ASSERT_EQUAL(expected, actual)`
- `TEST_ASSERT_TRUE(condition)`
- `TEST_ASSERT_FALSE(condition)`
- `TEST_ASSERT_NULL(pointer)`
- `TEST_ASSERT_NOT_NULL(pointer)`
- `TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)`

## Test Configuration

Configure test settings in `platformio.ini`:

```ini
[env:test]
platform = native
test_framework = unity
test_build_src = yes
```

## Best Practices

1. **Test Early and Often** - Write tests as you develop features
2. **One Assertion Per Test** - Keep tests focused and simple
3. **Use Descriptive Names** - Test names should describe what they test
4. **Test Edge Cases** - Include boundary conditions and error cases
5. **Mock Hardware** - Use mocks for hardware-dependent code when testing on native platform
6. **Clean Up** - Always clean up resources in tearDown()

## Documentation

For more information:
- [PlatformIO Unit Testing](https://docs.platformio.org/en/latest/advanced/unit-testing/)
- [Unity Test Framework](http://www.throwtheswitch.org/unity)
